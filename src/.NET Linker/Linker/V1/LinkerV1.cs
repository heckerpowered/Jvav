using System.Reflection.Emit;
using System.Text;
using Mono.Cecil;
using Mono.Cecil.Cil;

using OpCodes = Mono.Cecil.Cil.OpCodes;
using OpCode = Mono.Cecil.Cil.OpCode;
using System.Reflection;
using TypeAttributes = Mono.Cecil.TypeAttributes;
using MethodAttributes = Mono.Cecil.MethodAttributes;

namespace Jvav.Linker.V1;

public class Linking
{
    private readonly BinaryReader reader;
    private readonly AssemblyDefinition assembly;
    private readonly TypeDefinition programType;

    private readonly List<AssemblyDefinition> references = [];
    private readonly MethodReference printlnMethod;
    private readonly AssemblyDefinition mscorlib;
    public Linking(Stream inputStream)
    {
        reader = new(inputStream);
        ModuleName = ReadModuleName();

        mscorlib = AssemblyDefinition.ReadAssembly("mscorlib.dll");
        references.Add(mscorlib);

        var assemblyName = new AssemblyNameDefinition(ModuleName, new Version(1, 0, 0, 0));
        assembly = AssemblyDefinition.CreateAssembly(assemblyName, ModuleName, ModuleKind.Console);

        var objectType = ResolveType("System.Object")!;
        programType = new TypeDefinition("", "Program", TypeAttributes.Abstract | TypeAttributes.Sealed, objectType);
        assembly.MainModule.Types.Add(programType);

        printlnMethod = ResolveMethod("System.Console", "WriteLine", ["System.Object"])!;

        try
        {
            ReadMembers();
        }
        catch (EndOfStreamException) { };
    }

    public string ModuleName { get; private set; }

    private MethodReference? ResolveMethod(string typeName, string methodName, string[] parameterTypeNames)
    {
        var foundTypes = references
            .SelectMany(a => a.Modules)
            .SelectMany(m => m.Types)
            .Where(t => t.FullName == typeName)
            .ToArray();
        if (foundTypes.Length != 1)
        {
            return null;
        }

        var foundType = foundTypes.First();
        var methods = foundType.Methods.Where(method => method.Name == methodName);

        foreach (var method in methods)
        {
            if (method.Parameters.Count != parameterTypeNames.Length)
            {
                continue;
            }

            var allParametersMatch = true;

            for (var i = 0; i < parameterTypeNames.Length; i++)
            {
                if (method.Parameters[i].ParameterType.FullName != parameterTypeNames[i])
                {
                    allParametersMatch = false;
                    break;
                }
            }

            if (!allParametersMatch)
            {
                continue;
            }

            return assembly.MainModule.Import(method);
        }

        return null;
    }

    TypeReference? ResolveType(string metadataName)
    {
        var foundTypes = references.SelectMany(a => a.Modules)
                                   .SelectMany(m => m.Types)
                                   .Where(t => t.FullName == metadataName)
                                   .ToArray();
        if (foundTypes.Length == 1)
        {
            var typeReference = assembly.MainModule.Import(foundTypes.First());
            return typeReference;
        }

        return null;
    }

    private string ReadModuleName()
    {
        var moduleNameLength = reader.ReadUInt64();
        var bytes = reader.ReadBytes((int)moduleNameLength);
        var moduleName = Encoding.UTF8.GetString(bytes);
        return moduleName;
    }

    private Identifier ReadMemberType()
    {
        var memberType = (Identifier)reader.ReadByte();
        return memberType;
    }

    private void ReadMembers()
    {
        while (true)
        {
            var memberType = ReadMemberType();
            switch (memberType)
            {
                case Identifier.Function:
                    ReadFunction();
                    break;
                default:
                    Console.WriteLine($"Unknown member type: {memberType}");
                    return;
            }
        }
    }

    private Identifier ReadIdentifier()
    {
        var identifier = (Identifier)reader.ReadByte();
        return identifier;
    }

    private OpCode ReadOpCode()
    {
        var opCode = (InternalOpCodes)reader.ReadInt32();
        return opCode switch
        {
            InternalOpCodes.Add => OpCodes.Add,
            InternalOpCodes.AddOvf => OpCodes.Add_Ovf,
            InternalOpCodes.AddOvfUn => OpCodes.Add_Ovf_Un,
            InternalOpCodes.And => OpCodes.And,
            InternalOpCodes.Arglist => OpCodes.Arglist,
            InternalOpCodes.Beq => OpCodes.Beq,
            InternalOpCodes.BeqS => OpCodes.Beq_S,
            InternalOpCodes.Bge => OpCodes.Bge,
            InternalOpCodes.BgeS => OpCodes.Bge_S,
            InternalOpCodes.BgeUn => OpCodes.Bge_Un,
            InternalOpCodes.BgeUnS => OpCodes.Bge_Un_S,
            InternalOpCodes.Bgt => OpCodes.Bgt,
            InternalOpCodes.BgtS => OpCodes.Bgt_S,
            InternalOpCodes.BgtUn => OpCodes.Bgt_Un,
            InternalOpCodes.BgtUnS => OpCodes.Bgt_Un_S,
            InternalOpCodes.Ble => OpCodes.Ble,
            InternalOpCodes.BleS => OpCodes.Ble_S,
            InternalOpCodes.BleUn => OpCodes.Ble_Un,
            InternalOpCodes.BleUnS => OpCodes.Ble_Un_S,
            InternalOpCodes.Blt => OpCodes.Blt,
            InternalOpCodes.BltS => OpCodes.Blt_S,
            InternalOpCodes.BltUn => OpCodes.Blt_Un,
            InternalOpCodes.BltUnS => OpCodes.Blt_Un_S,
            InternalOpCodes.BneUn => OpCodes.Bne_Un,
            InternalOpCodes.BneUnS => OpCodes.Bne_Un_S,
            InternalOpCodes.Box => OpCodes.Box,
            InternalOpCodes.Br => OpCodes.Br,
            InternalOpCodes.BrS => OpCodes.Br_S,
            InternalOpCodes.Break => OpCodes.Break,
            InternalOpCodes.Brfalse => OpCodes.Brfalse,
            InternalOpCodes.BrfalseS => OpCodes.Brfalse_S,
            InternalOpCodes.Brtrue => OpCodes.Brtrue,
            InternalOpCodes.BrtrueS => OpCodes.Brtrue_S,
            InternalOpCodes.Call => OpCodes.Call,
            InternalOpCodes.Calli => OpCodes.Calli,
            InternalOpCodes.Callvirt => OpCodes.Callvirt,
            InternalOpCodes.Castclass => OpCodes.Castclass,
            InternalOpCodes.Ceq => OpCodes.Ceq,
            InternalOpCodes.Cgt => OpCodes.Cgt,
            InternalOpCodes.CgtUn => OpCodes.Cgt_Un,
            InternalOpCodes.Ckfinite => OpCodes.Ckfinite,
            InternalOpCodes.Clt => OpCodes.Clt,
            InternalOpCodes.CltUn => OpCodes.Clt_Un,
            InternalOpCodes.Constrained => OpCodes.Constrained,
            InternalOpCodes.ConvI => OpCodes.Conv_I,
            InternalOpCodes.ConvI1 => OpCodes.Conv_I1,
            InternalOpCodes.ConvI2 => OpCodes.Conv_I2,
            InternalOpCodes.ConvI4 => OpCodes.Conv_I4,
            InternalOpCodes.ConvI8 => OpCodes.Conv_I8,
            InternalOpCodes.ConvOvfI => OpCodes.Conv_Ovf_I,
            InternalOpCodes.ConvOvfIUn => OpCodes.Conv_Ovf_I_Un,
            InternalOpCodes.ConvOvfI1 => OpCodes.Conv_Ovf_I1,
            InternalOpCodes.ConvOvfI1Un => OpCodes.Conv_Ovf_I1_Un,
            InternalOpCodes.ConvOvfI2 => OpCodes.Conv_Ovf_I2,
            InternalOpCodes.ConvOvfI2Un => OpCodes.Conv_Ovf_I2_Un,
            InternalOpCodes.ConvOvfI4 => OpCodes.Conv_Ovf_I4,
            InternalOpCodes.ConvOvfI4Un => OpCodes.Conv_Ovf_I4_Un,
            InternalOpCodes.ConvOvfI8 => OpCodes.Conv_Ovf_I8,
            InternalOpCodes.ConvOvfI8Un => OpCodes.Conv_Ovf_I8_Un,
            InternalOpCodes.ConvOvfU => OpCodes.Conv_Ovf_U,
            InternalOpCodes.ConvOvfUUn => OpCodes.Conv_Ovf_U_Un,
            InternalOpCodes.ConvOvfU1 => OpCodes.Conv_Ovf_U1,
            InternalOpCodes.ConvOvfU1Un => OpCodes.Conv_Ovf_U1_Un,
            InternalOpCodes.ConvOvfU2 => OpCodes.Conv_Ovf_U2,
            InternalOpCodes.ConvOvfU2Un => OpCodes.Conv_Ovf_U2_Un,
            InternalOpCodes.ConvOvfU4 => OpCodes.Conv_Ovf_U4,
            InternalOpCodes.ConvOvfu4Un => OpCodes.Conv_Ovf_U4_Un,
            InternalOpCodes.ConvOvfU8 => OpCodes.Conv_Ovf_U8,
            InternalOpCodes.ConvOvfU8Un => OpCodes.Conv_Ovf_U8_Un,
            InternalOpCodes.ConvRUn => OpCodes.Conv_R_Un,
            InternalOpCodes.ConvR4 => OpCodes.Conv_R4,
            InternalOpCodes.ConvR8 => OpCodes.Conv_R8,
            InternalOpCodes.ConvU => OpCodes.Conv_U,
            InternalOpCodes.ConvU1 => OpCodes.Conv_U1,
            InternalOpCodes.ConvU2 => OpCodes.Conv_U2,
            InternalOpCodes.ConvU4 => OpCodes.Conv_U4,
            InternalOpCodes.ConvU8 => OpCodes.Conv_U8,
            InternalOpCodes.Cpblk => OpCodes.Cpblk,
            InternalOpCodes.Cpobj => OpCodes.Cpobj,
            InternalOpCodes.Div => OpCodes.Div,
            InternalOpCodes.DivUn => OpCodes.Div_Un,
            InternalOpCodes.Dup => OpCodes.Dup,
            InternalOpCodes.Endfilter => OpCodes.Endfilter,
            InternalOpCodes.Endfinally => OpCodes.Endfinally,
            InternalOpCodes.Initblk => OpCodes.Initblk,
            InternalOpCodes.Initobj => OpCodes.Initobj,
            InternalOpCodes.Isinst => OpCodes.Isinst,
            InternalOpCodes.jmp => OpCodes.Jmp,
            InternalOpCodes.Ldarg => OpCodes.Ldarg,
            InternalOpCodes.Ldarg0 => OpCodes.Ldarg_0,
            InternalOpCodes.Ldarg1 => OpCodes.Ldarg_1,
            InternalOpCodes.Ldarg2 => OpCodes.Ldarg_2,
            InternalOpCodes.Ldarg3 => OpCodes.Ldarg_3,
            InternalOpCodes.LdargS => OpCodes.Ldarg_S,
            InternalOpCodes.Ldarga => OpCodes.Ldarga,
            InternalOpCodes.LdargaS => OpCodes.Ldarga_S,
            InternalOpCodes.LdcI4 => OpCodes.Ldc_I4,
            InternalOpCodes.LcdI40 => OpCodes.Ldc_I4_0,
            InternalOpCodes.LdcI41 => OpCodes.Ldc_I4_1,
            InternalOpCodes.LdcI42 => OpCodes.Ldc_I4_2,
            InternalOpCodes.LdcI43 => OpCodes.Ldc_I4_3,
            InternalOpCodes.LdcI44 => OpCodes.Ldc_I4_4,
            InternalOpCodes.LdcI45 => OpCodes.Ldc_I4_5,
            InternalOpCodes.LdcI46 => OpCodes.Ldc_I4_6,
            InternalOpCodes.LdcI47 => OpCodes.Ldc_I4_7,
            InternalOpCodes.LdcI48 => OpCodes.Ldc_I4_8,
            InternalOpCodes.LdcI4M1 => OpCodes.Ldc_I4_M1,
            InternalOpCodes.LdcI4S => OpCodes.Ldc_I4_S,
            InternalOpCodes.LdcI8 => OpCodes.Ldc_I8,
            InternalOpCodes.LdcR4 => OpCodes.Ldc_R4,
            InternalOpCodes.LdcR8 => OpCodes.Ldc_R8,
            InternalOpCodes.Ldelem => OpCodes.Ldelem_Any,
            InternalOpCodes.LdelemI => OpCodes.Ldelem_I,
            InternalOpCodes.LdelemI1 => OpCodes.Ldelem_I1,
            InternalOpCodes.LdelemI2 => OpCodes.Ldelem_I2,
            InternalOpCodes.LdelemI4 => OpCodes.Ldelem_I4,
            InternalOpCodes.LdelemI8 => OpCodes.Ldelem_I8,
            InternalOpCodes.LdelemR4 => OpCodes.Ldelem_R4,
            InternalOpCodes.LdelemR8 => OpCodes.Ldelem_R8,
            InternalOpCodes.LdelemRef => OpCodes.Ldelem_Ref,
            InternalOpCodes.LdelemU1 => OpCodes.Ldelem_U1,
            InternalOpCodes.LdelemU2 => OpCodes.Ldelem_U2,
            InternalOpCodes.LdelemU4 => OpCodes.Ldelem_U4,
            InternalOpCodes.Ldelema => OpCodes.Ldelema,
            InternalOpCodes.Ldfld => OpCodes.Ldfld,
            InternalOpCodes.Ldflda => OpCodes.Ldflda,
            InternalOpCodes.Ldftn => OpCodes.Ldftn,
            InternalOpCodes.LdindI => OpCodes.Ldind_I,
            InternalOpCodes.LdindI1 => OpCodes.Ldind_I1,
            InternalOpCodes.LdindI2 => OpCodes.Ldind_I2,
            InternalOpCodes.LdindI4 => OpCodes.Ldind_I4,
            InternalOpCodes.LdindI8 => OpCodes.Ldind_I8,
            InternalOpCodes.LdindR4 => OpCodes.Ldind_R4,
            InternalOpCodes.LdindR8 => OpCodes.Ldind_R8,
            InternalOpCodes.LdindRef => OpCodes.Ldind_Ref,
            InternalOpCodes.LdindU1 => OpCodes.Ldind_U1,
            InternalOpCodes.LdindU2 => OpCodes.Ldind_U2,
            InternalOpCodes.LdindU4 => OpCodes.Ldind_U4,
            InternalOpCodes.Ldlen => OpCodes.Ldlen,
            InternalOpCodes.Ldloc => OpCodes.Ldloc,
            InternalOpCodes.Ldloc0 => OpCodes.Ldloc_0,
            InternalOpCodes.Ldloc1 => OpCodes.Ldloc_1,
            InternalOpCodes.Ldloc2 => OpCodes.Ldloc_2,
            InternalOpCodes.Ldloc3 => OpCodes.Ldloc_3,
            InternalOpCodes.LdlocS => OpCodes.Ldloc_S,
            InternalOpCodes.Ldloca => OpCodes.Ldloca,
            InternalOpCodes.LdlocaS => OpCodes.Ldloca_S,
            InternalOpCodes.Ldnull => OpCodes.Ldnull,
            InternalOpCodes.Ldobj => OpCodes.Ldobj,
            InternalOpCodes.Ldsfld => OpCodes.Ldsfld,
            InternalOpCodes.Ldsflda => OpCodes.Ldsflda,
            InternalOpCodes.Ldstr => OpCodes.Ldstr,
            InternalOpCodes.Ldtoken => OpCodes.Ldtoken,
            InternalOpCodes.Ldvirtftn => OpCodes.Ldvirtftn,
            InternalOpCodes.Leave => OpCodes.Leave,
            InternalOpCodes.LeaveS => OpCodes.Leave_S,
            InternalOpCodes.Localloc => OpCodes.Localloc,
            InternalOpCodes.Mkrefany => OpCodes.Mkrefany,
            InternalOpCodes.Mul => OpCodes.Mul,
            InternalOpCodes.MulOvf => OpCodes.Mul_Ovf,
            InternalOpCodes.MulOvfUn => OpCodes.Mul_Ovf_Un,
            InternalOpCodes.Neg => OpCodes.Neg,
            InternalOpCodes.Newarr => OpCodes.Newarr,
            InternalOpCodes.Newobj => OpCodes.Newobj,
            InternalOpCodes.Nop => OpCodes.Nop,
            InternalOpCodes.Not => OpCodes.Not,
            InternalOpCodes.Or => OpCodes.Or,
            InternalOpCodes.Pop => OpCodes.Pop,
            InternalOpCodes.Readonly => OpCodes.Readonly,
            InternalOpCodes.Refanytype => OpCodes.Refanytype,
            InternalOpCodes.Refanyval => OpCodes.Refanyval,
            InternalOpCodes.Rem => OpCodes.Rem,
            InternalOpCodes.RemUn => OpCodes.Rem_Un,
            InternalOpCodes.Ret => OpCodes.Ret,
            InternalOpCodes.Rethrow => OpCodes.Rethrow,
            InternalOpCodes.Shl => OpCodes.Shl,
            InternalOpCodes.Shr => OpCodes.Shr,
            InternalOpCodes.ShrUn => OpCodes.Shr_Un,
            InternalOpCodes.Sizeof => OpCodes.Sizeof,
            InternalOpCodes.Starg => OpCodes.Starg,
            InternalOpCodes.StargS => OpCodes.Starg_S,
            InternalOpCodes.Stelem => OpCodes.Stelem_Any,
            InternalOpCodes.StelemI => OpCodes.Stelem_I,
            InternalOpCodes.StelemI1 => OpCodes.Stelem_I1,
            InternalOpCodes.StelemI2 => OpCodes.Stelem_I2,
            InternalOpCodes.StelemI4 => OpCodes.Stelem_I4,
            InternalOpCodes.StelemI8 => OpCodes.Stelem_I8,
            InternalOpCodes.StelemR4 => OpCodes.Stelem_R4,
            InternalOpCodes.StelemR8 => OpCodes.Stelem_R8,
            InternalOpCodes.StelemRef => OpCodes.Stelem_Ref,
            InternalOpCodes.Stfld => OpCodes.Stfld,
            InternalOpCodes.StindI => OpCodes.Stind_I,
            InternalOpCodes.StindI1 => OpCodes.Stind_I1,
            InternalOpCodes.StintI2 => OpCodes.Stind_I2,
            InternalOpCodes.StindI4 => OpCodes.Stind_I4,
            InternalOpCodes.StindI8 => OpCodes.Stind_I8,
            InternalOpCodes.StindR4 => OpCodes.Stind_R4,
            InternalOpCodes.StintR8 => OpCodes.Stind_R8,
            InternalOpCodes.StindRef => OpCodes.Stind_Ref,
            InternalOpCodes.Stloc => OpCodes.Stloc,
            InternalOpCodes.Stloc0 => OpCodes.Stloc_0,
            InternalOpCodes.Stloc1 => OpCodes.Stloc_1,
            InternalOpCodes.Stloc2 => OpCodes.Stloc_2,
            InternalOpCodes.Stloc3 => OpCodes.Stloc_3,
            InternalOpCodes.StlocS => OpCodes.Stloc_S,
            InternalOpCodes.Stobj => OpCodes.Stobj,
            InternalOpCodes.Stsfld => OpCodes.Stsfld,
            InternalOpCodes.Sub => OpCodes.Sub,
            InternalOpCodes.SubOvf => OpCodes.Sub_Ovf,
            InternalOpCodes.SubOvfUn => OpCodes.Sub_Ovf_Un,
            InternalOpCodes.Switch => OpCodes.Switch,
            InternalOpCodes.Tailcall => OpCodes.Tail,
            InternalOpCodes.Throw => OpCodes.Throw,
            InternalOpCodes.Unaligned => OpCodes.Unaligned,
            InternalOpCodes.Unbox => OpCodes.Unbox,
            InternalOpCodes.UnboxAny => OpCodes.Unbox_Any,
            InternalOpCodes.Volatile => OpCodes.Volatile,
            InternalOpCodes.Xor => OpCodes.Xor,
            _ => throw new NotImplementedException(),
        };
    }
    private Identifier PeekIdentifier()
    {
        var position = reader.BaseStream.Position;
        var identifier = ReadIdentifier();
        reader.BaseStream.Position = position;
        return identifier;
    }
    private void ReadFunction()
    {
        var functionTypeName = ReadString();
        var functionName = ReadString();
        Console.WriteLine($"Linking function {functionName}");
        var functionParameterCount = reader.ReadUInt64();
        for (ulong i = 0; i < functionParameterCount; i++)
        {
            ReadString();
        }

        var method = new MethodDefinition(functionName, MethodAttributes.Public | MethodAttributes.Static, assembly.MainModule.TypeSystem.Void);
        programType.Methods.Add(method);
        var processor = method.Body.GetILProcessor();

        try
        {
            while (true)
            {
                var identifier = ReadIdentifier();
                if (identifier != Identifier.Instruction)
                {
                    break;
                }

                var opCode = ReadOpCode();
                if (opCode == OpCodes.Call)
                {
                    ReadIdentifier();
                    EmitCall(processor, ReadString());
                    continue;
                }
                switch (PeekIdentifier())
                {
                    case Identifier.String:
                        ReadIdentifier();
                        var stringValue = ReadString();
                        processor.Emit(opCode, stringValue);
                        break;
                    default:
                        break;
                }
            }
        }
        catch (EndOfStreamException) { }
        processor.Emit(OpCodes.Ret);

        if (functionName == "main")
        {
            assembly.EntryPoint = method;
        }
    }

    private void EmitCall(ILProcessor processor, string methodName)
    {
        // Built-in functions
        if (methodName == "println")
        {
            processor.Emit(OpCodes.Call, printlnMethod);
        }
    }

    private string ReadString()
    {
        var stringLength = reader.ReadUInt64();
        var bytes = reader.ReadBytes((int)stringLength);
        var value = Encoding.UTF8.GetString(bytes);
        return value;
    }

    public void Export()
    {
        assembly.Write(ModuleName);
    }
}
